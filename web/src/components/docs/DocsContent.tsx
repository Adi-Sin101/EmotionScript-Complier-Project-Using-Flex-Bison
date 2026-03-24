import type { ReactNode } from "react";

type DocsContentProps = {
  children: ReactNode;
};

export default function DocsContent({ children }: DocsContentProps) {
  return (
    <main id="docs-content" className="min-w-0 flex-1 px-5 py-8 sm:px-8 lg:px-10" tabIndex={-1}>
      <div className="mx-auto w-full max-w-4xl">{children}</div>
    </main>
  );
}
