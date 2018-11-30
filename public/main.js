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

test = () => {
    fetch("http://api.ipify.org?format=json")
        .then(res => console.log(res))
}