console.log('Bot is starting'); // checks if it's running when calling via node bot.js

//var Twit = require('twitter-api-v2'); // import statement
/*const { TwitterApi } = require('twitter-api-v2');
const config = require('./config');
const userClient = new TwitterApi(config);
const client = new TwitterApi('AAAAAAAAAAAAAAAAAAAAAIlOYwEAAAAAOVpmx79BRg%2FjXio0zD55iV%2BMlZs%3DZnGcwoYQngFD7QEAhrF9dDpLzHfRkYfLheajm85QXkNLnDKtAH');
var params = { q: 'banana since:2020-09-01', count: 10 }

TwitterApi.get('search/tweets', params, gotData);

function gotData(err, data, response) {
    console.log(data);
};*/



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

//set up a new stream
var stream = T.stream('statuses/filter', { follow: users })
stream.on('tweet', function(tweet) {
    if (!users.includes(tweet.user.id_str)) { return; }
    console.log(tweet.user.screen_name + ':' + tweet.text);
});


//var params = { q: 'fraise since:2020-09-01', count: 10 }

//T.get('search/tweets', params, gotData);

//function gotData(err, data, response) {
// var tweets = data.statuses;
// for (var i = 0; i < tweets.length; i++) {
//    console.log(tweets[i].text);
// }
//}
//setInterval(tweetThat, 1000 * 60 * 10);
//tweetThat();

function tweetThat() {
    var tweet = { status: 'Yet another bot test      #codingtrain' }
    T.post('statuses/update', tweet, tweeted);

    function tweeted(err, data, responde) {
        console
    }
}