import React, { useState } from "react";
import PropertyList from "../components/PropertyList";
import ConfirmDialog from "../components/ConfirmDialog";
import * as repo from "../data/indexedDbRepository";

export default function ListView({ properties, goToCreate, goToEdit, onPropertyDeleted }) {
  const [deleteId, setDeleteId] = useState(null);
  const [error, setError] = useState("");
  const [isDeleting, setIsDeleting] = useState(false);

  const confirmDelete = async () => {
    if (!deleteId) return;

    try {
      setIsDeleting(true);
      setError("");

      // Delete property
      await repo.remove(deleteId);

      if (onPropertyDeleted) {
        onPropertyDeleted(deleteId);
      }

      setDeleteId(null);
    } catch (err) {
      const errorMessage = err?.message || "Failed to delete property";
      console.error("Error deleting property:", err);
      setError(errorMessage);
    } finally {
      setIsDeleting(false);
    }
  };

  return (
    <>
      <button onClick={goToCreate}>+ Add Property</button>

      {error && (
        <div className="error" style={{ margin: "10px 0" }}>
          Error: {error}
          <button
            onClick={() => setError("")}
            style={{ marginLeft: "10px", padding: "2px 8px" }}
          >
            Dismiss
          </button>
        </div>
      )}

      <PropertyList
        properties={properties}
        onEdit={goToEdit}
        onDelete={(id) => {
          setError("");
          setDeleteId(id);
        }}
      />

      {deleteId && (
        <ConfirmDialog
          text={
            isDeleting
              ? "Deleting property..."
              : "Are you sure you want to delete this property?"
          }
          onConfirm={confirmDelete}
          onCancel={() => {
            setDeleteId(null);
            setError("");
          }}
          disabled={isDeleting}
        />
      )}
    </>
  );
}
