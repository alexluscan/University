import React, { useState } from "react";
import PropertyForm from "../components/PropertyForm";
import * as repo from "../data/indexedDbRepository";

export default function EditView({ property, goBack, onPropertyUpdated }) {
  const [error, setError] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);

  const update = async (updated) => {
    try {
      setIsSubmitting(true);
      setError("");

      // Update property
      const updatedProperty = await repo.update(updated);

      // Repository observer will notify subscribers automatically
      if (onPropertyUpdated) {
        onPropertyUpdated(updatedProperty);
      }

      goBack();
    } catch (err) {
      const errorMessage = err?.message || "Failed to update property";
      console.error("Error updating property:", err);
      setError(errorMessage);
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="card">
      <h2>Edit Property</h2>
      {error && <div className="error">Error: {error}</div>}
      <PropertyForm
        initial={property}
        onSubmit={update}
        onCancel={goBack}
        disabled={isSubmitting}
      />
    </div>
  );
}
