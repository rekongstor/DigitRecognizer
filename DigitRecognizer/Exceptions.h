#pragma once

class CorruptedFileEX : std::exception {};
class RendererEX : std::exception {};
class RendererDX11EX : RendererEX {};

class InvalidMatrixMul : std::exception {};