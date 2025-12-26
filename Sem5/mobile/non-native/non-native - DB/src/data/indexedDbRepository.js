/**
 * IndexedDB Repository for Property Management
 */

const DB_NAME = 'PropertyDB';
const DB_VERSION = 1;
const STORE_NAME = 'properties';

// Observer pattern
class RepositoryObserver {
  constructor() {
    this.subscribers = new Set();
  }

  subscribe(callback) {
    this.subscribers.add(callback);
    return () => this.subscribers.delete(callback);
  }

  notify(data) {
    this.subscribers.forEach(callback => {
      try {
        callback(data);
      } catch (error) {
        console.error('Error in observer callback:', error);
      }
    });
  }
}

const observer = new RepositoryObserver();

let dbInstance = null;
let propertiesCache = null;
let isInitialized = false;

function openDB() {
  return new Promise((resolve, reject) => {
    if (dbInstance) {
      resolve(dbInstance);
      return;
    }

    const request = indexedDB.open(DB_NAME, DB_VERSION);

    request.onerror = () => {
      const error = new Error(`Failed to open database: ${request.error}`);
      console.error('Database open error:', error, request.error);
      reject(error);
    };

    request.onsuccess = () => {
      dbInstance = request.result;
      resolve(dbInstance);
    };

    request.onupgradeneeded = (event) => {
      const db = event.target.result;
      if (!db.objectStoreNames.contains(STORE_NAME)) {
        const objectStore = db.createObjectStore(STORE_NAME, {
          keyPath: 'id',
          autoIncrement: true
        });
        objectStore.createIndex('title', 'title', { unique: false });
      }
    };
  });
}

async function getDB() {
  if (!dbInstance) {
    await openDB();
  }
  return dbInstance;
}

function logError(operation, error, context = {}) {
  const errorMessage = error?.message || String(error);
  const logEntry = {
    timestamp: new Date().toISOString(),
    operation,
    error: errorMessage,
    context
  };
  console.error(`[Repository Error] ${operation}:`, logEntry);
  return errorMessage;
}

/**
 * READ: Get all properties
 */
export async function getAll() {
  if (propertiesCache !== null && isInitialized) {
    return [...propertiesCache];
  }

  try {
    const db = await getDB();
    const transaction = db.transaction([STORE_NAME], 'readonly');
    const objectStore = transaction.objectStore(STORE_NAME);

    return new Promise((resolve, reject) => {
      const request = objectStore.getAll();

      request.onsuccess = () => {
        propertiesCache = request.result || [];
        isInitialized = true;
        resolve([...propertiesCache]);
      };

      request.onerror = () => {
        const error = new Error(`Failed to retrieve properties: ${request.error}`);
        const errorMessage = logError('getAll', error);
        reject(error);
      };
    });
  } catch (error) {
    const errorMessage = logError('getAll', error);
    throw new Error(`Error retrieving properties: ${errorMessage}`);
  }
}

export async function initialize() {
  try {
    await getAll();
  } catch (error) {
    logError('initialize', error);
    throw error;
  }
}

/**
 * CREATE: Add a new property
 */
export async function add(property) {
  try {
    const db = await getDB();
    const transaction = db.transaction([STORE_NAME], 'readwrite');
    const objectStore = transaction.objectStore(STORE_NAME);

    // Remove id from property if it exists, let DB generate it
    const propertyWithoutId = { ...property };
    delete propertyWithoutId.id;

    return new Promise((resolve, reject) => {
      const request = objectStore.add(propertyWithoutId);

      request.onsuccess = () => {
        try {
          const generatedId = request.result;
          const newProperty = { ...propertyWithoutId, id: generatedId };
          
          propertiesCache = [...(propertiesCache || []), newProperty];
          // Notify observers
          observer.notify(propertiesCache);
          resolve(newProperty);
        } catch (error) {
          const errorMessage = logError('add', error, { property });
          reject(new Error(`Error after creating property: ${errorMessage}`));
        }
      };

      request.onerror = () => {
        const error = new Error(`Failed to create property: ${request.error}`);
        const errorMessage = logError('add', error, { property });
        reject(error);
      };
    });
  } catch (error) {
    const errorMessage = logError('add', error, { property });
    throw new Error(`Error creating property: ${errorMessage}`);
  }
}

/**
 * UPDATE: Update an existing property
 */
export async function update(property) {
  if (!property || !property.id) {
    const error = new Error('Property ID is required for update');
    logError('update', error, { property });
    throw error;
  }

  try {
    const db = await getDB();
    const transaction = db.transaction([STORE_NAME], 'readwrite');
    const objectStore = transaction.objectStore(STORE_NAME);

    return new Promise((resolve, reject) => {
      const getRequest = objectStore.get(property.id);

      getRequest.onsuccess = () => {
        if (!getRequest.result) {
          const error = new Error(`Property with ID ${property.id} not found`);
          logError('update', error, { propertyId: property.id });
          reject(error);
          return;
        }

        const putRequest = objectStore.put(property);

        putRequest.onsuccess = () => {
          propertiesCache = (propertiesCache || []).map(p =>
            p.id === property.id ? property : p
          );
          // Notify observers
          observer.notify(propertiesCache);
          resolve(property);
        };

        putRequest.onerror = () => {
          const error = new Error(`Failed to update property: ${putRequest.error}`);
          const errorMessage = logError('update', error, { property });
          reject(error);
        };
      };

      getRequest.onerror = () => {
        const error = new Error(`Failed to retrieve property for update: ${getRequest.error}`);
        const errorMessage = logError('update', error, { propertyId: property.id });
        reject(error);
      };
    });
  } catch (error) {
    const errorMessage = logError('update', error, { property });
    throw new Error(`Error updating property: ${errorMessage}`);
  }
}

/**
 * DELETE: Remove a property by ID
 */
export async function remove(id) {
  if (!id) {
    const error = new Error('Property ID is required for deletion');
    logError('remove', error);
    throw error;
  }

  try {
    const db = await getDB();
    const transaction = db.transaction([STORE_NAME], 'readwrite');
    const objectStore = transaction.objectStore(STORE_NAME);

    return new Promise((resolve, reject) => {
      const getRequest = objectStore.get(id);

      getRequest.onsuccess = () => {
        if (!getRequest.result) {
          const error = new Error(`Property with ID ${id} not found`);
          logError('remove', error, { propertyId: id });
          reject(error);
          return;
        }

        const deleteRequest = objectStore.delete(id);

        deleteRequest.onsuccess = () => {
          propertiesCache = (propertiesCache || []).filter(p => p.id !== id);
          // Notify observers
          observer.notify(propertiesCache);
          resolve(id);
        };

        deleteRequest.onerror = () => {
          const error = new Error(`Failed to delete property: ${deleteRequest.error}`);
          const errorMessage = logError('remove', error, { propertyId: id });
          reject(error);
        };
      };

      getRequest.onerror = () => {
        const error = new Error(`Failed to retrieve property for deletion: ${getRequest.error}`);
        const errorMessage = logError('remove', error, { propertyId: id });
        reject(error);
      };
    });
  } catch (error) {
    const errorMessage = logError('remove', error, { propertyId: id });
    throw new Error(`Error deleting property: ${errorMessage}`);
  }
}


export function subscribe(callback) {
  return observer.subscribe(callback);
}


export function clearCache() {
  propertiesCache = null;
  isInitialized = false;
}

