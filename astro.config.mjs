// @ts-check
import { defineConfig } from 'astro/config';

// https://astro.build/config
export default defineConfig({
    site: "https://shynesogorb.github.io/",
    // base: "./",
    build: { assets: "assets" },
    vite: {
        build: {
            rollupOptions: {
                output: {
                    entryFileNames: '[name]-[hash].js',
                },
            },
        },
    },
});
