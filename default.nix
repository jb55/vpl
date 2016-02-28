{ stdenv, nanovg, glfw3, premake4, mesa_glu, glew, pkgconfig, stb }:
stdenv.mkDerivation rec {
  name = "vpl";
  version = "0.1";

  src = ./.;

  makeFlags = "PREFIX=$(out)";

  buildInputs = [ nanovg glfw3 premake4 mesa_glu glew pkgconfig stb ];

  meta = with stdenv.lib; {
    description = "Visual Programming Language";
    homepage = "https://github.com/jb55/vpl";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
  };
}
