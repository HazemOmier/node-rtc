describe('Browser side behavior', function() {
  it('Should complete handshake', function(done) {
    var ws = new WebSocket('ws://localhost:8080');
    ws.onmessage = function(event) {
      console.log(event);
      done();
    };
    ws.send('tette');
  });
});

