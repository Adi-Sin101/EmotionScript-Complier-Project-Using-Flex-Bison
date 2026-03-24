"use client";

import { memo, useEffect, useRef } from "react";
import Editor, { type Monaco, type OnChange } from "@monaco-editor/react";

type EditorDiagnostic = {
  kind: "lexical" | "syntax" | "semantic" | "runtime" | "unknown";
  message: string;
  line?: number;
};

type CodeEditorPanelProps = {
  externalCode: string;
  externalCodeVersion: number;
  onChange: (value: string) => void;
  onCursorChange?: (line: number, column: number) => void;
  diagnostics?: EditorDiagnostic[];
  jumpToLine?: number | null;
  onJumpHandled?: () => void;
};

let emotionscriptLanguageRegistered = false;

function registerEmotionscriptLanguage(monaco: Monaco) {
  if (emotionscriptLanguageRegistered) {
    return;
  }

  monaco.languages.register({ id: "emotionscript" });

  monaco.languages.setMonarchTokensProvider("emotionscript", {
    tokenizer: {
      root: [
        [/\$\$.*$/, "comment"],
        [/\$\*/, "comment", "@commentBlock"],
        [/"([^"\\]|\\.)*"/, "string"],
        [/-?\d+(\.\d+)?/, "number"],
        [/:->|:=|==|!=|<=|>=|\*\*|[+\-*/<>]/, "operator"],
        [/[{}()[\],.:]/, "delimiter"],
        [/\b(mind|awake|sleep|abort|scene|count|measure|truth|words|emotion|level|thought|memory|trait|state|set|speak|listen|alert|declare|end_declare|returns|return|call|if_feel|else_if_feel|else_feel|end_feel|decide|when|otherwise|end_decision|while_feeling|calm|for|break|continue|states|event|current|transition|persona|end_persona|open|guarded|hidden|static|inherit|override|public|protected|this|abs|sqrt|ceil|floor|diff|norm|pow|sin|cos|tan|asin|acos|atan|and|or|not|xor|true|false|interpret|reflect|invoke|replay|end_ruminate|evolves_from|reshape|IO_SPEAK|IO_LISTEN|IO_ALERT|manifest|composed_of|surge|trigger|balance|break_free|continue_flow|magnitude|normalize|arcsine|arccosine|arctangent|estimate_up|estimate_down|ruminate|shared|yes|no)\b/, "keyword"],
        [/[A-Za-z_][A-Za-z0-9_]*/, "identifier"],
        [/\s+/, "white"],
      ],
      commentBlock: [
        [/[^*$]+/, "comment"],
        [/\*\$/, "comment", "@pop"],
        [/[*$]/, "comment"],
      ],
    },
  });

  monaco.editor.defineTheme("emotionscript-theme", {
    base: "vs-dark",
    inherit: true,
    rules: [
      { token: "keyword", foreground: "8ab4ff" },
      { token: "string", foreground: "71d8ad" },
      { token: "number", foreground: "f4c97b" },
      { token: "comment", foreground: "6b7396" },
      { token: "operator", foreground: "f8d189" },
      { token: "identifier", foreground: "d0d5ef" },
    ],
    colors: {
      "editor.background": "#090b1b",
      "editorLineNumber.foreground": "#58608a",
      "editorLineNumber.activeForeground": "#9ea9dc",
      "editor.selectionBackground": "#2a356a66",
      "editorCursor.foreground": "#cdd6ff",
    },
  });

  emotionscriptLanguageRegistered = true;
}

function CodeEditorPanelComponent({
  externalCode,
  externalCodeVersion,
  onChange,
  onCursorChange,
  diagnostics = [],
  jumpToLine,
  onJumpHandled,
}: CodeEditorPanelProps) {
  const editorRef = useRef<Parameters<NonNullable<React.ComponentProps<typeof Editor>["onMount"]>>[0] | null>(null);
  const monacoRef = useRef<Monaco | null>(null);

  const handleEditorChange: OnChange = (value) => {
    onChange(value ?? "");
  };

  function handleBeforeMount(monaco: Monaco) {
    registerEmotionscriptLanguage(monaco);
  }

  function handleMount(editor: Parameters<NonNullable<React.ComponentProps<typeof Editor>["onMount"]>>[0], monaco: Monaco) {
    editorRef.current = editor;
    monacoRef.current = monaco;

    editor.onDidChangeCursorPosition((event) => {
      onCursorChange?.(event.position.lineNumber, event.position.column);
    });

    monaco.editor.setTheme("emotionscript-theme");
  }

  useEffect(() => {
    const editor = editorRef.current;
    const monaco = monacoRef.current;
    if (!editor || !monaco) {
      return;
    }

    const model = editor.getModel();
    if (!model) {
      return;
    }

    const markers = diagnostics.map((item) => {
      const startLineNumber = Math.max(1, item.line ?? 1);
      return {
        severity: monaco.MarkerSeverity.Error,
        message: item.message,
        startLineNumber,
        startColumn: 1,
        endLineNumber: startLineNumber,
        endColumn: model.getLineMaxColumn(startLineNumber),
      };
    });

    monaco.editor.setModelMarkers(model, "emotionscript", markers);
  }, [diagnostics]);

  useEffect(() => {
    const editor = editorRef.current;
    if (!editor) {
      return;
    }

    const currentValue = editor.getValue();
    if (currentValue !== externalCode) {
      editor.setValue(externalCode);
    }
  }, [externalCode, externalCodeVersion]);

  useEffect(() => {
    if (!jumpToLine || !editorRef.current) {
      return;
    }

    const editor = editorRef.current;
    const targetLine = Math.max(1, jumpToLine);
    editor.revealLineInCenter(targetLine);
    editor.setPosition({ lineNumber: targetLine, column: 1 });
    editor.focus();
    onJumpHandled?.();
  }, [jumpToLine, onJumpHandled]);

  return (
    <section className="flex h-full min-h-88 flex-col bg-[#090b1b]" aria-label="Code editor panel">
      <div className="flex h-10 items-center gap-2 border-b border-white/10 px-4 text-xs uppercase tracking-[0.08em] text-[#8a91b6]">
        <svg width="12" height="12" viewBox="0 0 24 24" fill="none" aria-hidden="true">
          <path d="M8 3h8l5 5v13H3V3h5Z" stroke="currentColor" strokeWidth="1.6" strokeLinejoin="round" />
          <path d="M16 3v5h5" stroke="currentColor" strokeWidth="1.6" strokeLinejoin="round" />
        </svg>
        <span>main.ems</span>
      </div>

      <div className="min-h-0 flex-1">
        <Editor
          height="100%"
          defaultLanguage="emotionscript"
          beforeMount={handleBeforeMount}
          defaultValue={externalCode}
          onChange={handleEditorChange}
          onMount={handleMount}
          options={{
            minimap: { enabled: false },
            fontSize: 14,
            fontFamily: "var(--font-jetbrains-mono)",
            lineHeight: 24,
            roundedSelection: false,
            scrollBeyondLastLine: false,
            padding: { top: 14, bottom: 14 },
            automaticLayout: true,
            wordWrap: "on",
            cursorSmoothCaretAnimation: "on",
            smoothScrolling: true,
            renderWhitespace: "selection",
            quickSuggestions: {
              other: true,
              comments: false,
              strings: false,
            },
          }}
        />
      </div>
    </section>
  );
}

export const CodeEditorPanel = memo(CodeEditorPanelComponent);
