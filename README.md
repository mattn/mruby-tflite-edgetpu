# mruby-tflite-edgetpu

EdgeTPU interface to TensorFlow Lite EdgeTPU for mruby

## Usage

```ruby
options = TfLite::InterpreterOptions.new
options.add_delegate TfLite::EdgeTPU.new(TfLite::EdgeTPU.devices[0])
model = TfLite::Model.from_file 'mobilenet_ssd_v2_coco_quant_postprocess_edgetpu.tflite'
interpreter = TfLite::Interpreter.new(model, options)
```

## Requirements

* TensorFlow Lite
* Coral EdgeTPU
* mruby-tflite

## License

MIT

## Author

Yasuhiro Matsumoto (a.k.a. mattn)
