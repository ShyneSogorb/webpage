import fs from 'fs';
import path from 'path';

export function getAllImages() {
    const imagesDir = 'public/images';
    console.log(imagesDir);
    
    const folders = fs.readdirSync(imagesDir);
    const images = {};

    folders.forEach(folder => {
        const folderPath = path.join(imagesDir, folder);
        if (fs.lstatSync(folderPath).isDirectory()) {
            const files = fs.readdirSync(folderPath);

            images[folder] = files.map(file => `images/${folder}/${file}`);
        }
    });
    
    return images;
}