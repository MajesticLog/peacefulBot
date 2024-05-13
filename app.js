// import the installed npm modules

const safeStringify = require("fast-safe-stringify");
const express = require("express");
const webSocket = require("ws");

// have a http server and a ws server
// the http one uses express - setup like so:

const app = express();
const server = require("http").createServer(app);
const webSocketPort = 3000;

// the websocket uses ws and is set up like so:

const ws = new webSocket.Server({
    port: webSocketPort,
    autoAcceptConnections: true,
});
server.listen(80);

// now tell the http server to send files from our folder
// this is where we put our p5.js etc sketches
app.use(express.static("BOT_test"));

// print the address/ console log the address that we need for our ESP later on
require("dns").lookup(require("os").hostname(),
    function(err, add, fam) {
        console.log("Your webSocket IP Address is:" + add + webSocketPort);
    });

// a variable for the connection to the controller
let controllerConnection = "";
let board1 = "";

// this event happens when a device connects to the ws server
ws.on("connection", function(connection) {
    // function we call when connection from the ESP or the p5.js sketch
    connection.on("message", function(message) {
        // parse the message to change it from a string to an object
        msgObj = JSON.parse(message);
        console.log(msgObj);

        // if statements to see where it came from. if from the control panel:
        if (msgObj.device == "CONTROL PANEL") {
            // check the action to see what's next
            if (msgObj.action == "INIT") {
                console.log("Controller connected");
                //save the connection to our variable
                controllerConnection = connection;
            }
            // if returns getRandom, send back a sketch
            if (msgObj.action == "getRandom") {
                console.log("Returning random");
                connection.send(
                    safeStringify({ action: "sendRandom", value: Math.random() })
                );
            }

            if (msgObj.action == "toggleLED") {
                console.log("Toggling LED");
                if (board1 != "") {
                    board1.send(safeStringify({ action: "toggleLED" }));
                }
            }
            // if the device ID is board1, various have various options

        }

        if (msgObj.device == "board1") {
            //check action
            if (msgObj.action == "INIT") {
                console.log("board1 connected");
                //save to our variable
                board1 = connection;
            }
            if (msgObj.action == "setPotentiometer") {
                console.log("Board1:" + msgObj.value);
                //send to the controller
                controllerConnection.send(
                    safeStringify({
                        action: "potentiometerValue",
                        value: msgObj.value,
                    })
                );
            }
        }
    });
});