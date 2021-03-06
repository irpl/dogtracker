const express  = require('express');
const app = express ();

const mongoose = require('mongoose');
const bodyParser = require("body-parser");
const path = require('path');

Data = require("./models/Data");

app.use(bodyParser.json());
app.use((req, res, next) => {
    if (req.is('text/*')) {
        req.text = '';
        req.setEncoding('utf-8');
        req.on('data', (chunk) => {req.text+=chunk;});
        req.on('end', next);
    } else {
        next();
    }
})
app.use(express.static(path.join(__dirname,"public")));


//connecting to mongo server
mongoose
    .connect ('mongodb://DogTracker:dogtracker1!@ds115244.mlab.com:15244/gps',{useNewUrlParser:true})
    .then(()=> console.log("Connected to mongo server"));


app.get("/", (req,res)=>{
    app.send("/index.html");
    console.log("Someone has accessed GET/...")

})
// Get keyword (asking the server for data)
app.get("/data",(req,res) =>{ 

    Data
        .find({})
        .sort({date:'desc'})
        .then((x)=>res.json(x));
    //res.send("You Tried to access get");
    // see on the terminal everytime someone asccess get

});

app.get("/recentdata",(req,res) =>{ 

    Data
    .find({})
    .sort({date:'desc'})
    .limit(1)
    .then((x)=>res.json(x[0]))
})

isInRange=(latitude,longitude)=>{
// compare lat and long with a or 
    latitude = parseFloat(latitude)
    longitude = parseFloat(longitude)

    var comparison;
    if (longitude < -76.748809 || longitude > -76.749145){
        return 'false@'
    }

    return 'true@';
}

// Post Keyword ( adds data --  posting information to the server )
app.post("/data", (req,res)=>{
    var data= req.text;
    data = data.trim();
    [atcom,info]=data.split(" ");
    var array= info.split(",");
    var latitude=array[3];
    var longitude=array[4];
    console.log(data);

    let response = isInRange(latitude, longitude)

    var newData = new Data({
        latitude: latitude,
        longitude: longitude
    });

    newData.save((err) => {
        if (err)res.send("error@")
        else res.send(response)
    })        

    
    
  
})

// Put Keyword (Updates data -- New value to update previous value)

app.listen(5000, () => {console.log("The server has started")});

