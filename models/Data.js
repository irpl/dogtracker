const mongoose = require ("mongoose");

const dataSchema = mongoose.Schema({
    latitude: {
        required:true , 
        type:Number
    },
    longitude: {
        required : true,
        type : Number
    },
    date: {
        type: Date,
        default: Date.now
    }
});


const Data = module.exports = mongoose.model("Data",dataSchema );