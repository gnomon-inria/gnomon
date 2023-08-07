(self["webpackChunkexample_client_webpack"] = self["webpackChunkexample_client_webpack"] || []).push([["src_client_lpy_js"],{

/***/ "./src/client/lpy.js":
/*!***************************!*\
  !*** ./src/client/lpy.js ***!
  \***************************/
/***/ ((__unused_webpack___webpack_module__, __webpack_exports__, __webpack_require__) => {

__webpack_require__.r(__webpack_exports__);
/* harmony export */ __webpack_require__.d(__webpack_exports__, {
/* harmony export */   "conf": () => (/* binding */ conf),
/* harmony export */   "language": () => (/* binding */ language)
/* harmony export */ });
/* harmony import */ var monaco_editor_esm_vs_editor_editor_api_js__WEBPACK_IMPORTED_MODULE_0__ = __webpack_require__(/*! monaco-editor/esm/vs/editor/editor.api.js */ "../../../node_modules/monaco-editor/esm/vs/editor/editor.api.js");
/*!-----------------------------------------------------------------------------
 * tristan cabel (tristan.cabel@inria.fr
 * March 2023
 *-----------------------------------------------------------------------------*/

//import { registerLanguage } from "monaco-editor/esm/vs/basic-languages/_.contribution.js";

var __defProp = Object.defineProperty;
var __getOwnPropDesc = Object.getOwnPropertyDescriptor;
var __getOwnPropNames = Object.getOwnPropertyNames;
var __hasOwnProp = Object.prototype.hasOwnProperty;
var __copyProps = (to, from, except, desc) => {
  if (from && typeof from === "object" || typeof from === "function") {
    for (let key of __getOwnPropNames(from))
      if (!__hasOwnProp.call(to, key) && key !== except)
        __defProp(to, key, { get: () => from[key], enumerable: !(desc = __getOwnPropDesc(from, key)) || desc.enumerable });
  }
  return to;
};
var __reExport = (target, mod, secondTarget) => (__copyProps(target, mod, "default"), secondTarget && __copyProps(secondTarget, mod, "default"));

// src/fillers/monaco-editor-core.ts
var monaco_editor_core_exports = {};
__reExport(monaco_editor_core_exports, monaco_editor_esm_vs_editor_editor_api_js__WEBPACK_IMPORTED_MODULE_0__);


// src/basic-languages/python/python.ts
var conf = {
  comments: {
    lineComment: "#",
    blockComment: ["'''", "'''"]
  },
  brackets: [
    ["{", "}"],
    ["[", "]"],
    ["(", ")"]
  ],
  autoClosingPairs: [
    { open: "{", close: "}" },
    { open: "[", close: "]" },
    { open: "(", close: ")" },
    { open: '"', close: '"', notIn: ["string"] },
    { open: "'", close: "'", notIn: ["string", "comment"] }
  ],
  surroundingPairs: [
    { open: "{", close: "}" },
    { open: "[", close: "]" },
    { open: "(", close: ")" },
    { open: '"', close: '"' },
    { open: "'", close: "'" }
  ],
  onEnterRules: [
    {
      beforeText: new RegExp("^\\s*(?:def|class|for|if|elif|else|while|try|with|finally|except|async|match|case).*?:\\s*$"),
      action: { indentAction: monaco_editor_core_exports.languages.IndentAction.Indent }
    }
  ],
  folding: {
    offSide: true,
    markers: {
      start: new RegExp("^\\s*#region\\b"),
      end: new RegExp("^\\s*#endregion\\b")
    }
  }
};
var language = {
  defaultToken: "",
  tokenPostfix: ".lpy",
  keywords: ["False", "None", "True", "and", "as", "assert", "async", "await", "break", "continue", "del", "elif", "else", "except", "exec", "finally", "for", "from", "if", "import", "in", "is", "nonlocal", "not", "or", "pass", "print", "raise", "return", "try", "while", "with", "yield", "hex", "abs", "all", "any", "apply", "basestring", "bin",  "buffer", "callable", "chr", "classmethod", "cmp", "coerce", "compile", "complex", "delattr", "dir", "divmod", "enumerate", "eval", "execfile", "file", "filter", "format", "frozenset", "getattr", "globals", "hasattr", "hash", "help", "id", "input", "intern", "isinstance", "issubclass", "iter", "len", "locals", "max", "memoryview", "min", "next", "object", "oct", "open", "ord", "pow", "print", "property", "reversed", "range", "raw_input", "reduce", "reload", "repr", "reversed", "round", "self", "set", "setattr", "slice", "sorted", "staticmethod", "str", "sum", "super", "tuple", "type", "unichr", "unicode", "vars", "xrange", "zip", "__dict__", "__methods__", "__members__", "__class__", "__bases__", "__name__", "__mro__", "__subclasses__", "__init__", "__import__"],
  datatype: ["bool", "bytearray", "dict", "int", "list", "map", "float", "long", "complex"],
  defclass: ['def', 'class', 'global', 'lambda'],
  lpykeywords: ["Axiom", "production", "homomorphism", "interpretation", "decomposition", "endlsystem", "group", "endgroup" , "nsproduce", "makestring", "consider", "ignore", "forward", "backward", "isForward", "extern", "Start", "End", "StartEach", "EndEach", "getGroup", "useGroup", "getIterationNb", "-static->", "lpyimport"],
  lpylinekeywords: ["module", "produce", "nproduce"],
  brackets: [
    { open: "{", close: "}", token: "delimiter.curly" },
    { open: "[", close: "]", token: "delimiter.bracket" },
    { open: "(", close: ")", token: "delimiter.parenthesis" }
  ],
  tokenizer: {
    root: [
      { include: "@whitespace" },
      { include: "@numbers" },
      { include: "@strings" },
      [/[,:;]/, "delimiter"],
      [/[{}\[\]()]/, "@brackets"],
      [/@[a-zA-Z_]\w*/, "tag"],
      [/^[A-Z_]\w*\( *([^)]+?) *\)/, "type.identifier"], // lpy functions
      [/derivation length/, 'constant'],
      [/maximum depth/, 'constant'],
      [/[a-zA-Z_]\w*/,
       {
         // see https://microsoft.github.io/monaco-editor/playground.html?source=v0.36.1#example-extending-language-services-semantic-tokens-provider-example for color example
          cases: {
            "@keywords": "keyword",
            "@datatype": "keyword",
            "@defclass": "type.identifier",
            "@lpykeywords": "constant",
            "@lpylinekeywords": {token:"constant", next:"@lpyline"},
            "@default": "identifier"
          }
        }
      ],
      [/-->/, "constant"]
    ],
    whitespace: [
      [/\s+/, "white"],
      [/(^#.*$)/, "comment"],
      [/'''/, "string", "@endDocString"],
      [/"""/, "string", "@endDblDocString"]
    ],
    endDocString: [
      [/[^']+/, "string"],
      [/\\'/, "string"],
      [/'''/, "string", "@popall"],
      [/'/, "string"]
    ],
    endDblDocString: [
      [/[^"]+/, "string"],
      [/\\"/, "string"],
      [/"""/, "string", "@popall"],
      [/"/, "string"]
    ],
    numbers: [
      [/-?0x([abcdef]|[ABCDEF]|\d)+[lL]?/, "number.hex"],
      [/-?(\d*\.)?\d+([eE][+\-]?\d+)?[jJ]?[lL]?/, "number"]
    ],
    strings: [
      [/'$/, "string.escape", "@popall"],
      [/'/, "string.escape", "@stringBody"],
      [/"$/, "string.escape", "@popall"],
      [/"/, "string.escape", "@dblStringBody"]
    ],
    stringBody: [
      [/[^\\']+$/, "string", "@popall"],
      [/[^\\']+/, "string"],
      [/\\./, "string"],
      [/'/, "string.escape", "@popall"],
      [/\\$/, "string"]
    ],
    dblStringBody: [
      [/[^\\"]+$/, "string", "@popall"],
      [/[^\\"]+/, "string"],
      [/\\./, "string"],
      [/"/, "string.escape", "@popall"],
      [/\\$/, "string"]
    ],
    lpyline: [
      [/.*/, 'regexp', '@pop']
    ]
  }
};


//registerLanguage({
//  id: "lpy",
//  extensions: [".lpy", ".py"],
//  aliases: ["Lpy", "lpy"],
//  loader: () => {conf, language}
//});


/***/ })

}])
//# sourceMappingURL=src_client_lpy_js.main.js.map