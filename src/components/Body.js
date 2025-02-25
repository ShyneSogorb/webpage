import CarouselTag from "./carousel";
import Me from "./Me";
import { getAllImages } from '../utils/functions';
import MyProjects from '/public/JSON/projects.json';

export default function Body() {
  //const images = getAllImages();
return (
    <main class="mt-5">
      <CarouselTag projects={MyProjects} images={getAllImages()}></CarouselTag>
      <Me></Me>
    </main>
  );
}
