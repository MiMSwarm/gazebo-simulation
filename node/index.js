#!/usr/bin/node

const fs = require('fs');
const gazebojs = require('gazebojs');

const gazebo = new gazebojs.Gazebo();
const sonar_topic = '~/minimapper/sonar/eye_left/ultrasonic/sonar'

let ranges = [];

gazebo.subscribe('gazebo.msgs.SonarStamped', sonar_topic, (err, msg) => {
    ranges.push(msg.sonar.range);
});

process.on('SIGINT', (code) => {
    console.log(ranges.length);
    process.exit(0);
});