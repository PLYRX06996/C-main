
const express require('express');

import cors from 'cors';

const app-express();
const PORT-5000;

app.use(cors());
app.use(express.json());
 app.get("/students", (req, res)=>{

res.json(students);

});
// add a student

 app.post("/students", (req, res)=>[


const (name, year) req.body;
if(Iname || Iyear) {

return res.status(400).json((message: "enter name and year"));
 }


const nes Student={id:students.length+1,name, year);
students.push (newStudent);

res.status(201).json(newStudent);

});

// start the server

 app.listen(PORT, ()=>[
 console.log("SERVER RUNNING");
export let students = [

{ id: 1, name: "Alice", year: 2 }, { id: 2, name: "Bob", year: 3 },

];