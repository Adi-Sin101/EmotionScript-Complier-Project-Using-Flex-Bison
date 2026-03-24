import FeatureCards from "@/components/FeatureCards";
import Footer from "@/components/Footer";
import HeroSection from "@/components/HeroSection";
import Navbar from "@/components/Navbar";
import PipelineSection from "@/components/PipelineSection";

export default function Home() {
  return (
    <div className="relative min-h-screen overflow-x-hidden bg-background-dark text-white">
      <div className="pointer-events-none fixed inset-0 -z-20 bg-[radial-gradient(circle_at_18%_-10%,rgba(76,97,255,0.26),transparent_40%),radial-gradient(circle_at_82%_0%,rgba(77,194,255,0.2),transparent_44%),linear-gradient(180deg,#0d111a_0%,#0f1421_42%,#0e131f_100%)]" />
      <div
        className="pointer-events-none fixed inset-0 -z-10 bg-cover bg-center opacity-8 mix-blend-screen blur-[1px]"
        style={{ backgroundImage: "url('https://cdn.esawebb.org/archives/images/screen/weic2205a.jpg')" }}
      />
      <div className="pointer-events-none subtle-grid fixed inset-0 -z-10 opacity-35" />
      <div className="pointer-events-none fixed inset-0 -z-10 bg-linear-to-b from-[#0f1323]/75 via-[#0f1424]/82 to-[#0e1320]/94" />

      <Navbar />
      <main>
        <HeroSection />
        <FeatureCards />
        <PipelineSection />
      </main>
      <Footer />
    </div>
  );
}
