const express = require('express')
const app = express()
const port = 3000
var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://192.168.0.108')

var flag = 0;

client.on('connect', function () {
  client.subscribe('presence', function (err) {
    if (!err) {
      console.log('connected')
      flag = 1;
    }
  })
})

app.use(express.json());

app.post('*', (req, res) => {
  console.log('post: '+req.url)
  console.log(req.body)
  var data = req.body;
  var stringMsg = ''+data.sensors[0].value0 +','+ data.sensors[0].value1 +','+ data.sensors[0].value2 + ',' + data.sensors[1].value0 +',' + data.sensors[2].value0 +','+ data.sensors[2].value1 +','+ data.sensors[2].value2
  client.publish('J7-prime/sensors', stringMsg)
  res.send()
})


app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
})