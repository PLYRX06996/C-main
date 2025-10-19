import { useState, useEffect } from 'react';
import './App.css';

function App() {
  // --- STATE MANAGEMENT ---
  // State for the form input
  const [name, setName] = useState('');
  // State for the theme, initialized from localStorage or defaults to 'theme-light'
  const [theme, setTheme] = useState(localStorage.getItem('theme') || 'theme-light');

  // --- EVENT HANDLERS ---
  const handleSubmit = (e) => {
    e.preventDefault();
    alert(`Hello, ${name}!`);
  };

  // Function to toggle between light and dark theme
  const toggleTheme = () => {
    const newTheme = theme === 'theme-light' ? 'theme-dark' : 'theme-light';
    setTheme(newTheme);
  };

  // --- SIDE EFFECTS ---
  // Use useEffect to update localStorage and the body class whenever the theme changes
  useEffect(() => {
    localStorage.setItem('theme', theme);
    // We apply the theme class to the body to make the CSS variables global
    document.body.className = theme;
  }, [theme]);

  // --- RENDER ---
  return (
    // The container div no longer needs a class, as the theme is on the body
    <div className="container">
      <h1>Theme Switcher</h1>
      {/* Correct React onClick event handler */}
      <button id="switch" onClick={toggleTheme}>
        Switch to {theme === 'theme-light' ? 'Dark' : 'Light'}
      </button>

      <div style={{ marginTop: '20px' }}>
        <h2>Basic Form</h2>
        <form onSubmit={handleSubmit}>
          <input
            type="text"
            placeholder="Enter your name"
            value={name}
            onChange={(e) => setName(e.target.value)}
          />
          <button type="submit">Submit</button>
        </form>
      </div>
    </div>
  );
}

export default App;
