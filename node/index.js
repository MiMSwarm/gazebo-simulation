const fs = require('fs');
const gazebojs = require('gazebojs');

let gazebo = new gazebojs.Gazebo();

let ranges = [];

let sonar_topic = '~/minimapper/sonar/eye_left/ultrasonic/sonar'
gazebo.subscribe('gazebo.msgs.SonarStamped', sonar_topic, (err, msg) => {
    console.log(msg.sonar);
});

function onExit() {
    fs.open('../data/sonar.txt');
}

// process.on('exit', )