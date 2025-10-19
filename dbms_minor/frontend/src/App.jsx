import { BrowserRouter as Router, Routes, Route, Link } from 'react-router-dom';
import { useState, useEffect } from 'react';

const defaultSubtasks = [
	'Buy groceries',
	'Finish report',
	'Call client',
	'Plan weekend trip',
	'Read a book',
	'Exercise',
	'Pay bills',
	'Clean the house'
];

function CreateTask() {
	const [date, setDate] = useState('');
	const [taskName, setTaskName] = useState('');
	const [showSubtasks, setShowSubtasks] = useState(false);
	const [selectedSubtasks, setSelectedSubtasks] = useState([]);
	const [subtaskChecks, setSubtaskChecks] = useState(Array(defaultSubtasks.length).fill(false));

	const handleSubtaskCheck = idx => {
		const updatedChecks = [...subtaskChecks];
		updatedChecks[idx] = !updatedChecks[idx];
		setSubtaskChecks(updatedChecks);
	};

	const handleOkay = () => {
		const selected = defaultSubtasks.filter((_, idx) => subtaskChecks[idx]);
		setSelectedSubtasks(selected);
		setShowSubtasks(false);
	};

		const [saveStatus, setSaveStatus] = useState('');

		const handleSave = async () => {
			if (!date || !taskName) {
				setSaveStatus('Please enter date and task name.');
				return;
			}
			const newTask = {
				id: Date.now(),
				date,
				name: taskName,
				subtasks: selectedSubtasks
			};
			setSaveStatus('Saving...');
			try {
				const res = await fetch('http://localhost:3001/tasks');
				const tasks = await res.json();
				const updatedTasks = [...tasks, newTask];
				const saveRes = await fetch('http://localhost:3001/tasks', {
					method: 'POST',
					headers: { 'Content-Type': 'application/json' },
					body: JSON.stringify(updatedTasks)
				});
				if (saveRes.ok) {
					setSaveStatus('Task saved!');
					setDate('');
					setTaskName('');
					setSelectedSubtasks([]);
					setSubtaskChecks(Array(defaultSubtasks.length).fill(false));
				} else {
					setSaveStatus('Failed to save task.');
				}
			} catch {
				setSaveStatus('Error saving task.');
			}
		};

			return (
					<div className="card" style={{ width: '100%', minHeight: '100vh' }}>
						<h2>Create Task</h2>
						<div className="input-group">
							<label className="input-label">Date:</label>
							<input className="input-field" type="date" value={date} onChange={e => setDate(e.target.value)} />
						</div>
						<div className="input-group">
							<label className="input-label">Task Name:</label>
							<input className="input-field" type="text" value={taskName} onChange={e => setTaskName(e.target.value)} placeholder="Enter task name" />
						</div>
						<button className="task-btn" type="button" onClick={() => setShowSubtasks(true)}>
							Add Subtasks
						</button>
						{showSubtasks && (
							<div className="card subtasks-list" style={{ background: '#f3f4f6', border: '1px solid #e0e7ff', borderRadius: 12 }}>
								<h4>Select Subtasks</h4>
								{defaultSubtasks.map((sub, idx) => (
									<div key={sub} style={{ marginBottom: 4 }}>
										<input
											type="checkbox"
											checked={subtaskChecks[idx]}
											onChange={() => handleSubtaskCheck(idx)}
											id={`subtask-${idx}`}
										/>
										<label htmlFor={`subtask-${idx}`}>{sub}</label>
									</div>
								))}
								<button className="task-btn" type="button" style={{ marginTop: '1rem' }} onClick={handleOkay}>Okay</button>
							</div>
						)}
						{selectedSubtasks.length > 0 && (
							<div className="subtasks-list">
								<h4>Selected Subtasks</h4>
								<ul>
									{selectedSubtasks.map(sub => (
										<li key={sub}>{sub}</li>
									))}
								</ul>
							</div>
						)}
						<button className="task-btn" type="button" onClick={handleSave}>Save Task</button>
						{saveStatus && <div className="status-message" style={{ color: saveStatus === 'Task saved!' ? 'green' : 'red' }}>{saveStatus}</div>}
					</div>
		);
}
function AllTasks() {
	const [tasks, setTasks] = useState([]);
	const [loading, setLoading] = useState(true);
	const [error, setError] = useState('');

	useEffect(() => {
		async function fetchTasks() {
			setLoading(true);
			setError('');
			try {
				const res = await fetch('http://localhost:3001/tasks');
				if (!res.ok) throw new Error('Failed to fetch tasks');
				const data = await res.json();
				setTasks(data);
			} catch {
				setError('Error fetching tasks');
			}
			setLoading(false);
		}
		fetchTasks();
	}, []);

	const grouped = tasks.reduce((acc, task) => {
		acc[task.date] = acc[task.date] || [];
		acc[task.date].push(task);
		return acc;
	}, {});

		const [deleteStatus, setDeleteStatus] = useState('');

		const handleDelete = async (taskId) => {
			setDeleteStatus('Deleting...');
			try {
				const updatedTasks = tasks.filter(task => task.id !== taskId);
				const res = await fetch('http://localhost:3001/tasks', {
					method: 'POST',
					headers: { 'Content-Type': 'application/json' },
					body: JSON.stringify(updatedTasks)
				});
				if (res.ok) {
					setTasks(updatedTasks);
					setDeleteStatus('Task deleted!');
				} else {
					setDeleteStatus('Failed to delete task.');
				}
			} catch {
				setDeleteStatus('Error deleting task.');
			}
			setTimeout(() => setDeleteStatus(''), 1500);
		};

			return (
					<div style={{ width: '100%', minHeight: '100vh' }}>
						<h2>All Tasks</h2>
						{loading && <div>Loading...</div>}
						{error && <div style={{ color: 'red' }}>{error}</div>}
						{deleteStatus && <div className="status-message" style={{ color: deleteStatus === 'Task deleted!' ? 'green' : 'red' }}>{deleteStatus}</div>}
						{!loading && !error && Object.keys(grouped).length === 0 && <div>No tasks found.</div>}
						{!loading && !error && Object.entries(grouped).map(([date, tasks]) => (
							<div key={date} style={{ marginBottom: '2rem', borderBottom: '1px solid #eee', paddingBottom: '1rem' }}>
								<h3 style={{ marginBottom: '0.5rem' }}>Date: {date}</h3>
								{tasks.map(task => (
									<div key={task.id} className="card" style={{ display: 'flex', flexDirection: 'column' }}>
										<strong>Task Name:</strong> {task.name}
										{task.subtasks && task.subtasks.length > 0 && (
											<div className="subtasks-list">
												<strong>Subtasks:</strong>
												<ul>
													{task.subtasks.map((sub, idx) => (
														<li key={idx}>{sub}</li>
													))}
												</ul>
											</div>
										)}
										<button
											className="delete-btn"
											onClick={() => handleDelete(task.id)}
										>Delete</button>
									</div>
								))}
							</div>
						))}
					</div>
			);
}

export default function App() {
	return (
		<Router>
			<div style={{ display: 'flex', flexDirection: 'column', alignItems: 'flex-start', padding: '2rem 0 0 2rem', minHeight: '100vh', alignContent: 'centre' }}>
				<nav style={{ display: 'flex', gap: '1rem', padding: '0.5rem 1rem', background: '#f0f0f0', borderRadius: '6px', marginBottom: '1.5rem' }}>
					<Link to="/create">Create Task</Link>
					<Link to="/tasks">All Tasks</Link>
				</nav>
				<div style={{ minWidth: '100%' }}>
					<Routes>
						<Route path="/create" element={<CreateTask />} />
						<Route path="/tasks" element={<AllTasks />} />
						<Route path="/" element={<CreateTask />} />
					</Routes>
				</div>
			</div>
		</Router>
	);
}
