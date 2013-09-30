var rtcModule = require('../build/Release/rtc.node');

var pc1 = new rtcModule.NodeRTCPeerconnection();
var pc2 = new rtcModule.NodeRTCPeerconnection();

pc1.onIceCandidate = function(event) {
  console.log("call1");
  console.log(event.candidate);
  pc2.addIceCandidate(event.candidate);
};

pc2.onIceCandidate = function(event) {
  console.log("call2");
  console.log(event.candidate);
  pc1.addIceCandidate(event.candidate);
};

var done = function() {
  console.log('Ends');
};

var onAnswer = function(desc) {
  pc2.setLocalDescription(desc);
  pc1.setRemoteDescription(desc, done);
};

var createAnswer = function() {
  pc2.createAnswer(onAnswer, function(error) { logMsg(error); });
};

var onOffer = function(desc) {
  pc1.setLocalDescription(desc);
  pc2.setRemoteDescription(desc, createAnswer, function(error) { logMsg(error); });
};

console.log(pc1);
console.log(pc2);
pc1.createOffer(onOffer, function(error) { console.log(error); });

