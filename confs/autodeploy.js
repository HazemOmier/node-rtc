var npm = require('npm');

npm.load({}, function(err) {
  npm.registry.adduser(process.argv[2], process.argv[3], process.argv[4], function(err) {
    if (err) {
      console.log(err);
    } else {
      npm.config.set('email', process.argv[4], 'user');
      npm.commands.publish([], function(err) {
        console.log(err || 'Published to registry');
      });
    }
  });
});
