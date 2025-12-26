# Real estate rental mobile app

### Non-Technical Overview

This mobile application is designed to make the process of renting real estate properties simple and convenient. As a potential client, you will be able to browse through a list of available properties, view important details such as price, location, and rental duration, and contact the property owner or agent directly through the app. If you’re a landlord, you can create property listings, update their information at any time, and remove them once they are no longer available.
The app offers a smooth experience whether you’re looking to rent or to list a property. All actions like viewing, adding, editing, and deleting can be performed easily without needing any technical knowledge.

### Problem Domain Overview

Entity that will be persisted: Property

Fields of the entity:

title: string — The title of the property listing (e.g., “2-Bedroom Apartment in Downtown”).

address: string — The full location of the property (street, city, postal code).

price: double — The monthly rental cost of the property in the chosen currency.

description: string — A short summary describing the property’s features.

propertyType: string — The type of property (e.g., Apartment, House, Studio, Commercial).

availabilityStatus: string — Indicates whether the property is currently available or rented.

contactInfo: string — The contact details of the property owner or agent.

startDate: date — The start date of the rental period.

endDate: date — The end date of the rental period.

### Actions – CRUD Operations

The application supports the following actions for managing properties:

Create/Add: Users can create and add new property listings with full details, including rental start and end dates, price, address, and contact information.

Read/View: Users can view the list of all available properties and see details like price, location, and rental period.

Update: Users can modify their property listings at any time (e.g. adjust the price, update contact info, or change the rental period).

Delete: Users can remove a property listing when it is no longer available or rented out.

### Persistence Details

All CRUD operations are persisted both on the local database and the remote server:

Create/Add: New property listings are stored locally and pushed to the server.

Update: Updates to listings—including changes to rental dates—are synchronized between the local database and the server.

Delete: Deleted listings are removed from both local storage and the server.

Read: Property data is retrieved from the local database and synchronized with the server to maintain consistency.

### Offline App Handling

The application allows basic functionality even without an internet connection. Offline behavior for each operation is as follows:

Create: New property listings, including rental dates, will be saved locally. Once the device reconnects, the listings are automatically pushed to the server.

Read: Previously loaded or stored listings (with all their details) will still be viewable from the local database, with a message indicating the app is offline.

Update: The app will notify the user that updates cannot be made while offline and give the option to retry once a connection is available.

Delete: The app will display a message indicating that deletion cannot be completed offline. The action can be retried after reconnection.

### App Mockup
You can check the initially proposed design of the app: 
https://www.figma.com/design/Pf6sZJE7c8fgmvt1ZCPQ9H/A1?node-id=2001-116
https://www.figma.com/design/Pf6sZJE7c8fgmvt1ZCPQ9H/A1?node-id=2001-590
