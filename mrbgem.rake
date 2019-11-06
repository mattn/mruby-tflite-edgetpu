MRuby::Gem::Specification.new('mruby-tflite-edgetpu') do |spec|
  spec.license = 'MIT'
  spec.authors = 'mattn'

  spec.linker.libraries << 'edgetpu'
end
