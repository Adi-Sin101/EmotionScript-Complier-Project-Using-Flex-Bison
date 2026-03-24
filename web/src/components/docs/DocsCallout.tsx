import type { ReactNode } from "react";

type DocsCalloutProps = {
  title: string;
  children: ReactNode;
};

export default function DocsCallout({ title, children }: DocsCalloutProps) {
  return (
    <aside className="my-8 border-l-4 border-primary bg-primary/5 px-5 py-4" aria-label={title}>
      <div className="mb-2 flex items-center gap-2">
        <span className="inline-flex h-2.5 w-2.5 rounded-full bg-primary" aria-hidden="true" />
        <p className="text-base font-semibold text-primary">{title}</p>
      </div>
      <div className="text-sm leading-7 text-white/75">{children}</div>
    </aside>
  );
}
