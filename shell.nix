with import <nixpkgs> {}; 
callPackage ./default.nix { 
  stdenv = clangStdenv;
}
