import FooterSection from "./footer/FooterSection";
import MySocials from "../../../public/JSON/social.json";
import MyProjects from "./../../../public/JSON/projects";

export default function Footer() { 
  return (
    <div class="mt-5 w-100">
    <footer class="row row-cols-1 row-cols-sm-2 row-cols-md-5 py-5 mt-5 border-top bg-dark text-white">
      <FooterSection Title="Social" Reference={MySocials}></FooterSection>
      <FooterSection Title="Projects" Reference={MyProjects}></FooterSection>
    </footer>
    </div>
  );
}
