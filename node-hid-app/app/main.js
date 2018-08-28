var hid = require('./HID.js');


function logDevices() {
    var devices = hid.getDevices();
    console.log(devices);
}

var write_loop = setInterval(hid.writeEWT, 2000);
hid.catchError(write_loop);
hid.readEWT();



