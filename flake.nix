{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      systems = [
        "x86_64-linux"
      ];

      forEachSystem = f: nixpkgs.lib.genAttrs systems (system: f pkgsFor.${system});

      pkgsFor = nixpkgs.lib.genAttrs systems (
        system:
        import nixpkgs {
          inherit system;
        }
      );
    in
    {
      formatter = forEachSystem (pkgs: pkgs.nixfmt-rfc-style);

      devShells = forEachSystem (pkgs: import ./shell.nix { inherit pkgs; });
    };
}
