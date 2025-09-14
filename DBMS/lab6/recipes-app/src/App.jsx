import React, { useEffect, useState } from "react";
import "./App.css";

function App() {
  const [recipes, setRecipes] = useState([]);
  const [loading, setLoading] = useState(true);
  const [selectedRecipe, setSelectedRecipe] = useState(null);

  useEffect(() => {
    fetch("https://dummyjson.com/recipes?limit=110&skip=0")
      .then((res) => res.json())
      .then((data) => {
        setRecipes(data.recipes);
        setLoading(false);
      });
  }, []);

  if (loading) return <h2 className="loading">Loading...</h2>;

  return (
    <div className="recipes-container">
      {recipes.map((recipe) => (
        <div key={recipe.id} className="recipe-card">
          <img src={recipe.image} alt={recipe.name} />
          <h3>{recipe.name}</h3>
          <p className="cuisine">{recipe.cuisine}</p>
          <button onClick={() => setSelectedRecipe(recipe)}>View Recipe</button>
        </div>
      ))}

      {selectedRecipe && (
        <div className="modal-overlay" onClick={() => setSelectedRecipe(null)}>
          <div
            className="modal"
            onClick={(e) => e.stopPropagation()} // Prevent closing when clicking inside
          >
            <button className="close-btn" onClick={() => setSelectedRecipe(null)}>
              ✖
            </button>
            <img src={selectedRecipe.image} alt={selectedRecipe.name} />
            <h2>{selectedRecipe.name}</h2>
            <p><strong>Cuisine:</strong> {selectedRecipe.cuisine}</p>
            <p><strong>Difficulty:</strong> {selectedRecipe.difficulty}</p>
            <p><strong>Prep Time:</strong> {selectedRecipe.prepTimeMinutes} min</p>
            <p><strong>Cook Time:</strong> {selectedRecipe.cookTimeMinutes} min</p>
            <p><strong>Servings:</strong> {selectedRecipe.servings}</p>
            <p><strong>calories Per Serving:</strong> {selectedRecipe.caloriesPerServing}</p>
            <p><strong>Ingredients:</strong></p>
            <p className="ingredients">{selectedRecipe.ingredients}</p>
            <p><strong>Instructions:</strong></p>
            <p className="instructions">{selectedRecipe.instructions}</p>
            <p><strong>Meal Type:</strong></p>
            <p className="mealType">{selectedRecipe.mealType}</p>
            <p><strong>Review Count:</strong> {selectedRecipe.reviewCount}</p>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;