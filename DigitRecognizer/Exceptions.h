#pragma once

class CorruptedFileEX : std::exception {};
class RendererEX : std::exception {};
class RendererDX11EX : RendererEX {};

class InvalidMatrixMul : std::exception {};
class InvalidMatrixAdd : std::exception {};
class InvalidMatrixResize : std::exception {};