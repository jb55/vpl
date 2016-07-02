{ stdenv, nanovg, SDL2, premake4, mesa_glu, glew, pkgconfig, stb, luajit, terra,
  haskellPackages
}:

let haskellEnv = haskellPackages.ghcWithPackages (pkgs: with pkgs; [
                   llvm-general safe QuickCheck
                 ]);
in stdenv.mkDerivation rec {
  name = "vpl";
  version = "0.1";

  src = ./.;

  makeFlags = "PREFIX=$(out)";

  buildInputs = [ nanovg SDL2 premake4 mesa_glu glew pkgconfig stb luajit haskellEnv terra ];

  meta = with stdenv.lib; {
    description = "Visual Programming Language";
    homepage = "https://github.com/jb55/vpl";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
  };
}
