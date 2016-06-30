with import <nixpkgs> {}; 
callPackage ./default.nix { 
  nanovg = callPackage ./var/nix/nanovg.nix { };
  stb    = callPackage ./var/nix/stb.nix { };
  stdenv = clangStdenv;
}
