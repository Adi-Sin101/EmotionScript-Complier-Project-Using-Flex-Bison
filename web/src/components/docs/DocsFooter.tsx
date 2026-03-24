import Link from "next/link";

export default function DocsFooter() {
  return (
    <footer className="mt-16 border-t border-white/10 py-8">
      <div className="mx-auto flex max-w-4xl flex-col gap-3 px-5 text-sm text-white/45 sm:flex-row sm:items-center sm:justify-between sm:px-8 lg:px-10">
        <p>&copy; 2024 EmotionScript Foundation. Built for expressive code.</p>
        <nav className="flex items-center gap-5" aria-label="Footer links">
          <Link href="#" className="transition-colors hover:text-white/75">
            Privacy
          </Link>
          <Link href="#" className="transition-colors hover:text-white/75">
            Terms
          </Link>
          <Link href="#" className="transition-colors hover:text-white/75">
            Security
          </Link>
        </nav>
      </div>
    </footer>
  );
}
