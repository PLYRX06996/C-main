const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const cors = require('cors');
app.use(cors());
const PORT = 3001;

app.use(express.json());

const TASKS_FILE = path.join(__dirname, 'tasks.json');

app.get('/tasks', (req, res) => {
  fs.readFile(TASKS_FILE, 'utf8', (err, data) => {
    if (err) {
      return res.status(500).json({ error: 'Failed to read tasks.' });
    }
    const tasks = JSON.parse(data || '[]');
    res.json(tasks);
  });
});

app.post('/tasks', (req, res) => {
  const tasks = req.body;
  if (!Array.isArray(tasks)) {
    return res.status(400).json({ error: 'Tasks should be an array.' });
  }
  fs.writeFile(TASKS_FILE, JSON.stringify(tasks, null, 2), 'utf8', err => {
    if (err) {
      return res.status(500).json({ error: 'Failed to save tasks.' });
    }
    res.json({ message: 'Tasks saved successfully.' });
  });
});

app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
