import React, { useState } from "react";
import PropertyForm from "../components/PropertyForm";
import * as repo from "../data/indexedDbRepository";

export default function CreateView({ goBack, onPropertyCreated }) {
  const [error, setError] = useState("");
  const [isSubmitting, setIsSubmitting] = useState(false);

  const create = async (prop) => {
    try {
      setIsSubmitting(true);
      setError("");

      // Create property
      const newProperty = await repo.add(prop);

      // Repository observer will notify subscribers automatically
      if (onPropertyCreated) {
        onPropertyCreated(newProperty);
      }

      goBack();
    } catch (err) {
      const errorMessage = err?.message || "Failed to create property";
      console.error("Error creating property:", err);
      setError(errorMessage);
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <div className="card">
      <h2>Add Property</h2>
      {error && <div className="error">Error: {error}</div>}
      <PropertyForm
        initial={null}
        onSubmit={create}
        onCancel={goBack}
        disabled={isSubmitting}
      />
    </div>
  );
}
