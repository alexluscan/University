import React, { useState, useEffect } from "react";
import ListView from "./views/ListView";
import CreateView from "./views/CreateView";
import EditView from "./views/EditView";
import * as repo from "./data/indexedDbRepository";

export default function App() {
  const [mode, setMode] = useState("list");
  const [editItem, setEditItem] = useState(null);
  const [properties, setProperties] = useState([]);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(true);

  // Load data once when the application starts
  useEffect(() => {
    let unsubscribe = null;

    const initializeData = async () => {
      try {
        setLoading(true);
        setError(null);
        
        // Initialize repository
        await repo.initialize();
        
        const initialProperties = await repo.getAll();
        setProperties(initialProperties);

        // Subscribe to repository changes using observer
        unsubscribe = repo.subscribe((updatedProperties) => {
          setProperties([...updatedProperties]);
        });
      } catch (err) {
        const errorMessage = err?.message || 'Failed to load properties';
        console.error('Error initializing repository:', err);
        setError(errorMessage);
      } finally {
        setLoading(false);
      }
    };

    initializeData();

    return () => {
      if (unsubscribe) {
        unsubscribe();
      }
    };
  }, []);

  const handlePropertyDeleted = (id) => {
  };

  const handlePropertyCreated = (newProperty) => {
  };

  const handlePropertyUpdated = (updatedProperty) => {
  };

  if (loading) {
    return (
      <div className="app-container">
        <h1>Non-Native CRUD - Rent Properties</h1>
        <div>Loading properties...</div>
      </div>
    );
  }

  if (error) {
    return (
      <div className="app-container">
        <h1>Non-Native CRUD - Rent Properties</h1>
        <div className="error">Error loading properties: {error}</div>
        <button onClick={() => window.location.reload()}>Retry</button>
      </div>
    );
  }

  return (
    <div className="app-container">
      <h1>Non-Native CRUD - Rent Properties</h1>

      {mode === "list" && (
        <ListView
          properties={properties}
          goToCreate={() => setMode("create")}
          goToEdit={(prop) => {
            setEditItem(prop);
            setMode("edit");
          }}
          onPropertyDeleted={handlePropertyDeleted}
        />
      )}

      {mode === "create" && (
        <CreateView
          goBack={() => setMode("list")}
          onPropertyCreated={handlePropertyCreated}
        />
      )}

      {mode === "edit" && (
        <EditView
          property={editItem}
          goBack={() => setMode("list")}
          onPropertyUpdated={handlePropertyUpdated}
        />
      )}
    </div>
  );
}
