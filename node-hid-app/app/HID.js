var HID = require('node-hid');
var EWT = new HID.HID(1003, 9218);

module.exports.getDevices = function() {
    var devices = HID.devices();
    return devices;
}

module.exports.readEWT = function() {
    console.log("Reading Data...");
    EWT.on('data', function(data) {
        console.log("recieved data:", data);
    });

}

module.exports.writeEWT = function () {
    var data = [];
    for (i=0; i <= 64; i++) {
        data.push(i);
    }
    EWT.write(data);
    console.log("Writing to EWT");
    console.log(EWT.write(data));
    console.log("End of Transmission");
}

module.exports.getFeatureReport = function() {
    EWT.getFeatureReport();
}

module.exports.catchError = function(loop_id) {
    EWT.on('error', function(err) {
        console.log("error:", err);
        clearInterval(loop_id);
        EWT.close();
    });
}


