// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.core;

export import p.core.basic_block;
export import p.core.instruction;
export import p.core.context;
export import p.core.unit_loader;
export import p.core.symbol;
export import p.core.container_symbol;
export import p.core.program_symbol;
export import p.core.unit_symbol;
export import p.core.unit_part_symbol;
export import p.core.root_symbol;
export import p.core.symbol_table;
export import p.core.block_symbol;
export import p.core.writer;
export import p.core.reader;
export import p.core.type_symbol;
export import p.core.array_type_symbol;
export import p.core.object_type_symbol;
export import p.core.alias_type_symbol;
export import p.core.constant_symbol;
export import p.core.variable_symbol;
export import p.core.subroutine_symbol;
export import p.core.standard_subroutine;
export import p.core.type_resolver;
export import p.core.value;
export import p.core.parsing_context;
export import p.core.parser;
export import p.core.rt;
export import p.core.heap;
export import p.core.function_repository;
export import p.core.emitter;
export import p.core.vmt;
export import p.core.execution_context;
export import p.core.compile_flags;