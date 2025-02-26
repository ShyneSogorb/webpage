import fs from 'fs';
import path from 'path';
import projects from '../../public/JSON/projects.json'


export function getAllFirstImages()
{
    const imagesDir = 'public/images';
    const images = projects.map(project=>{
        
        const projectFolder = path.join(imagesDir, (project.short || project.name))

        console.log(projectFolder);
        if(fs.existsSync(projectFolder)){
            
            return path.join(projectFolder, fs.readdirSync(projectFolder)[0])
        }

    })

    console.log(images);
    
    return images;
}

export function getAllImages() {

    const imagesDir = 'public/images';

    const folders = fs.readdirSync(imagesDir);
    const images = {};

    folders.forEach(folder => {
        const folderPath = path.join(imagesDir, folder);
        if (fs.lstatSync(folderPath).isDirectory()) {
            const files = fs.readdirSync(folderPath);

            images[folder] = files.map(file => `images/${folder}/${file}`);
        }
    });
    
    console.log(images);
    

    return images;
}