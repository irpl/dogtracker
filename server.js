const express  = require('express');
const app = express ();

const mongoose = require('mongoose');
const bodyParser = require("body-parser");
const path = require('path');

Data = require("./models/Data");

app.use(bodyParser.json());
app.use(express.static(path.join(__dirname,"public")));


//connecting to mongo server
mongoose
    .connect ('mongodb://DogTracker:dogtracker1!@ds115244.mlab.com:15244/gps',{useNewUrlParser:true})
    .then(()=> console.log("Connected to mongo server"));


app.get("/", (req,res)=>{
    app.send("/index.html");
})
// Get keyword (asking the server for data)
app.get("/data",(req,res) =>{ 

    Data
        .find({})
        .sort({date:'desc'})
        .then((x)=>res.json(x));
    //res.send("You Tried to access get");
    // see on the terminal everytime someone asccess get
    console.log("Someone has accessed GET/...")

});

// Post Keyword ( adds data --  posting information to the server )

app.post("/data", (req,res)=>{
    var newData = new Data({
        latitude: req.body.latitude,
        longitude: req.body.longitude
    });

    newData
        .save()
        .then(res.json({success: true}))

})

// Put Keyword (Updates data -- New value to update previous value)

app.listen(5000, () => {console.log("The server has started")});
