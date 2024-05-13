// import the installed npm modules

var Twit = require('twit');
var config = require('./config');
var T = new Twit(config);

var users = ['1241098622969634817', //somegoodnews
    '22025046', // good news network
    '58448800', //positivenewsuk
    '4000812135', //happynewspaper_
    '2269369861', //goodnews_shared
    '766345628725182464', //goodnewsmoments
    '1408803128003158019', //goodnewsmoveme3
    '17064610', //HNheadlines
    '1453302663362273283', //moi
    '1291384381068783631' // lmpositif
];

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
app.use(express.static("public"));

// print the address/ console log the address that we need for our ESP later on
require("dns").lookup(require("os").hostname(),
    function(err, add, fam) {
        console.log("Your webSocket IP Address is:" + add + ":" + webSocketPort);
    });

// a variable for the connection to the controller
let controllerConnection = "";
let board1 = "";

//set up a new stream
var stream = T.stream('statuses/filter', { follow: users })
stream.on('tweet', function(tweet) {
    if (!users.includes(tweet.user.id_str)) { return; }

    board1.send(safeStringify({ action: "toggleLED" }));

    console.log(tweet.user.screen_name + ':' + tweet.text);
});

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
                if (board1 != "") {
                    board1.send(safeStringify({ action: "toggleLED" }));
                }
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

        }
    });
});