var express = require('express');
var app = express();
var Twitter = require('twitter');
var emojiStrip = require('emoji-strip');

var bodyParser = require("body-parser");

app.use(bodyParser.urlencoded({ extended: false }))

var client = new Twitter({
  consumer_key: '',
  consumer_secret: '',
  access_token_key: '',
  access_token_secret: ''
});

try {
var Discord = require('discord.js');
var discordclient = new Discord.Client();

var email = '';
var password = '';
discordclient.login(email, password);
var dhchannel;
discordclient.on("ready", function () {
  dhchannel =  discordclient.channels.get("");

});
} catch (e) {
console.log(e);
}

var brightness = 50;

/*channel.fetchMessages({limit: 1})
    .then(function(messages) {
      console.log("Received ${messages.size} messages")
    })
.catch(console.error);*/

app.get("/b", function(req, res) {
	res.send('<form method="POST"><input type="number" name="bright" value="' + brightness + '"/></form>');
});

app.post("/b", function(req, res) {
	brightness = req.body.bright;
	res.redirect("/b");
}); 

app.get("/", function(req, res) {
  console.log("Got connection");
  client.get('search/tweets', {q: 'dreamhack', count: 1, result_type: "recent"}, function(error, tweets, response) {
    var filtered = [];
    filtered = tweets.statuses.map(function(tweet) {
      var str = emojiStrip("@" + tweet.user.screen_name + ": " + tweet.text);
      str = str.replace(/[^A-Za-zåäöÅÄÖ 0-9 \.,\?""!@#\$%\^&\*\(\)-_=\+;:<>\/\\\|\}\{\[\]`~]*/g, '')
      return str;
    });

    dhchannel.fetchMessages({limit: 1}).then(function (messages) {
      var message = messages.array()[0];
      filtered.push("@" + message.author.username + ": " + message.cleanContent);
      res.send(brightness + "ffff" + filtered.join("||||"));
    });

  });
  return;
});


app.listen(process.env.PORT || 3000, process.env.IP || "0.0.0.0", function(){
  console.log("Chat server listening at", process.env.IP);
});
