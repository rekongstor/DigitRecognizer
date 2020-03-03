#pragma once

class CorruptedFileEX : std::exception {};
class RendererEX : std::exception {};
class RendererDX11EX : RendererEX {};

class MatrixOutOfRange : std::exception {};
class NotEnough_dL : std::exception {};

class InvalidMatrixMul : std::exception {};
class InvalidMatrixSMul : std::exception {};
class InvalidMatrixScalMul : std::exception {};
class InvalidMatrixRowMul : std::exception {};

class InvalidMatrixSumCol : std::exception {};
class InvalidMatrixSumRow : std::exception {};
class InvalidMatrixSum : std::exception {};
class InvalidMatrixAdd : std::exception {};
class InvalidMatrixResize : std::exception {};
class InvalidMatrixTranspSize : std::exception {};

class LayerNotFinalValue : std::exception {};
class InvalidTestRefs : std::exception {};