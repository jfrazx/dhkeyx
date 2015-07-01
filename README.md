
# DHKeyX

An implementation of the Diffie–Hellman key exchange for Ruby.

This is a fork of: https://rubygems.org/gems/dhkeyexchange for Ruby >= 1.9.2.

## INSTALLATION

  $[sudo] gem install dhkeyx

## USAGE

```
  require 'dhkeyx'

  dh = DHKey.new([base], [prime], [private_key])

  dh.base
  => "6521178014511969166738152222198510777"



```

### METHODS 
  'base=' => 'g='
  'prime=' => 'p='
  'private_key=' => 'a='
  'their_public_key' => 'y='

  'base' => 'g'
  'prime' => 'p'
  'private_key' => 'a'
  'my_public_key' => 'x'
  'their_public_key' => 'y'
  'shared_key' => 'k' 

