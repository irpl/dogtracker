var map = L.map('mapid').setView([18.004981, -76.748856], 13);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

var circle = L.circle([18.004801, -76.748993], {
    color: 'red',
    fillColor: '#f03',
    fillOpacity: 0.5,
    radius: 15
}).addTo(map);

// The marker that shows where the dog is at present time


setInterval(()=>{
    test();
}, 3000);

test = () => {
    fetch("http://philliplogan.com:5000/recentdata")
        .then(res => res.json())
        .then(data => {
            var marker = L.marker([data.latitude, data.longitude]).addTo(map);
        })
}