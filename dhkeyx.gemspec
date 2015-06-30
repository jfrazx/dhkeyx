# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'dhkeyx/version'

Gem::Specification.new do |spec|
  spec.name          = "dhkeyx"
  spec.version       = DHKeyX::VERSION
  spec.authors       = ["Simon Menke","jfrazx"]
  spec.email         = ["simon@3motions.net", "staringblind@gmail.com"]
  spec.summary       = %q{An implementation of the Diffie Hellman key exchange protocol}
  spec.description   = %q{}
  spec.platform      = Gem::Platform::RUBY
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = Dir.glob("ext/**/*.{c,rb}") + Dir.glob("lib/**/*.rb")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib", "ext"]

  spec.extensions    = ["ext/dhkeyx/extconf.rb"]

  spec.add_development_dependency "rake", "~> 10.4"
end
