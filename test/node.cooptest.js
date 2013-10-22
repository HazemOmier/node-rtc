var rtcModule = require('../build/Release/rtc.node');
var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({port: 8080});

wss.on('connection', function(ws) {
  var pc = new rtcModule.NodeRTCPeerconnection();

  ws.on('message', function(message) {
    ws.send(message);
  });
});

