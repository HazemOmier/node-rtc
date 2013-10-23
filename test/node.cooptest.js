var rtcModule = require('../build/Release/rtc.node');
var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({port: 8080});

wss.on('connection', function(ws) {
  var pc = new rtcModule.NodeRTCPeerconnection();

  var onAnswer = function(desc) {
      pc.setLocalDescription(desc);
      ws.send(JSON.stringify({ type: 'answer', body: desc }));
  };

  var createAnswer = function() {
    pc.createAnswer(onAnswer, function(error) { logMsg(error); });
  };

  ws.on('message', function(message) {
    message = JSON.parse(message);
    console.log('Received: ' + message.type);
    if (message.type === 'offer') {
      pc.setRemoteDescription(message.body, createAnswer, function(error) { logMsg(error); });
    }
    else {
      ws.send(JSON.stringify({ type: 'error', body: 'Wrong message received' }));
    }
  });
});

