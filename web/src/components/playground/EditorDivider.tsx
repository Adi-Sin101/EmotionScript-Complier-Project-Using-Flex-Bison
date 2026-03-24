import type { MouseEvent } from "react";

type EditorDividerProps = {
  onMouseDown: (event: MouseEvent<HTMLButtonElement>) => void;
};

export function EditorDivider({ onMouseDown }: EditorDividerProps) {
  return (
    <button
      type="button"
      aria-label="Resize panels"
      onMouseDown={onMouseDown}
      className="hidden w-1 cursor-col-resize bg-white/5 transition-colors hover:bg-[#2c2ce2] md:block"
    />
  );
}
