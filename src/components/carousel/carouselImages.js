'use client';

import Carousel from 'react-bootstrap/Carousel';

export default function CarouselProjectItem({project, image, first}) {
    console.log(project);
    // class="d-block mw-100 h-auto m-auto"
    return (
    <Carousel.Item class={first ? "active" : ""} key={image}>
        <img src={image} alt={project.name + " slide"}/>
        <Carousel.Caption>
        <h3>{`${project.name} (${project.type})`}</h3>
        <p>{project.shortDescription}</p>
        </Carousel.Caption>
    </Carousel.Item>
    )
}
