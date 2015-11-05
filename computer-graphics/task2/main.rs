extern crate image;
extern crate rand;

use image::{GenericImage, Pixel};

use std::fs::File;
use std::path::Path;
use std::env;

// A, B, C

fn pixel_average_value(src: image::Rgba<u8>) -> u8 {
    let ch = src.channels();
    let (r, g, b) = (ch[0] as u32, ch[1] as u32, ch[2] as u32);
    ((r + g + b) / 3) as u8
}

fn pixel_apply_threshold(src: image::Rgba<u8>, threshold: u8) -> image::Rgba<u8> {
    if pixel_average_value(src) < threshold {
        image::Rgba([0, 0, 0, src.channels()[3]])
    } else {
        image::Rgba([255, 255, 255, src.channels()[3]])
    }
}

fn pixel_round(src: image::Rgba<u8>) -> image::Rgba<u8> {
    pixel_apply_threshold(src, 128)
}

fn pixel_random_dither(src: image::Rgba<u8>) -> image::Rgba<u8> {
    let threshold = rand::random::<u8>();
    pixel_apply_threshold(src, threshold)
}

fn pixel_ordered_dither(src: image::Rgba<u8>, x: u32, y: u32) -> image::Rgba<u8> {
    let threshold = [
        [1.0 / 17.0, 9.0 / 17.0, 3.0 / 17.0, 11.0 / 17.0],
        [13.0 / 17.0, 5.0 / 17.0, 15.0 / 17.0, 7.0 / 17.0],
        [4.0 / 17.0, 12.0 / 17.0, 2.0 / 17.0, 10.0 / 17.0],
        [16.0 / 17.0, 8.0 / 17.0, 14.0 / 17.0, 6.0 / 17.0],
    ];
    pixel_apply_threshold(src, (threshold[(x % 4) as usize][(y % 4) as usize] * 256.0) as u8)
}

fn image_round(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    for x in 0..width {
        for y in 0..height {
            let rgba = img.get_pixel(x, y).to_rgba();
            img.put_pixel(x, y, pixel_round(rgba));
        }
    }
}

fn image_random_dither(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    for x in 0..width {
        for y in 0..height {
            let rgba = img.get_pixel(x, y).to_rgba();
            img.put_pixel(x, y, pixel_random_dither(rgba));
        }
    }
}

fn image_ordered_dither(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    for x in 0..width {
        for y in 0..height {
            let rgba = img.get_pixel(x, y).to_rgba();
            img.put_pixel(x, y, pixel_ordered_dither(rgba, x, y));
        }
    }
}

// D, E, F

fn value_apply_threshold(value: i32, threshold: i32) -> (u8, i32) {
    if value < threshold {
        (0, value - 0)
    } else {
        (255, value - 255)
    }
}

fn image_diffuse_forward(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    let mut errors = vec![vec![0 as i32; width as usize]; height as usize];

    for y in 0..height {
        for x in 0..width {
            let rgba = img.get_pixel(x, y).to_rgba();

            let curr = pixel_average_value(rgba) as i32 + errors[y as usize][x as usize];
            let (new, error) = value_apply_threshold(curr, 128 as i32);

            if x + 1 < width {
                errors[y as usize][x as usize + 1] = error;
            }

            img.put_pixel(x, y, image::Rgba([new, new, new, rgba.channels()[3]]));
        }
    }
}

fn image_diffuse_double(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    let mut errors = vec![vec![0 as i32; width as usize]; height as usize];

    for y in 0..height {
        let rng = if y % 2 == 0 {
            (0..width).collect::<Vec<u32>>()
        } else {
            (0..width).rev().collect::<Vec<u32>>()
        };
        for x in rng {
            let rgba = img.get_pixel(x, y).to_rgba();

            let curr = pixel_average_value(rgba) as i32 + errors[y as usize][x as usize];
            let (new, error) = value_apply_threshold(curr, 128 as i32);

            if y % 2 == 0 {
                if x + 1 < width {
                    errors[y as usize][x as usize + 1] = error;
                }
            } else {
                if x >= 1 {
                    errors[y as usize][x as usize - 1] = error;
                }
            }

            img.put_pixel(x, y, image::Rgba([new, new, new, rgba.channels()[3]]));
        }
    }
}

fn image_diffuse_floyd(img: &mut image::DynamicImage) {
    let (width, height) = img.dimensions();

    let mut errors = vec![vec![0 as i32; width as usize]; height as usize];

    for y in 0..height {
        let rng = if y % 2 == 0 {
            (0..width).collect::<Vec<u32>>()
        } else {
            (0..width).rev().collect::<Vec<u32>>()
        };
        for x in rng {
            let rgba = img.get_pixel(x, y).to_rgba();

            let curr = pixel_average_value(rgba) as i32 + errors[y as usize][x as usize];
            let (new, error) = value_apply_threshold(curr, 128 as i32);

            if y % 2 == 0 {
                if x + 1 < width {
                    errors[y as usize][x as usize + 1] += error * 5 / 16;
                }
                if y + 1 < height {
                    if x >= 1 {
                        errors[y as usize + 1][x as usize - 1] += error * 1 / 16;
                    }
                    errors[y as usize + 1][x as usize] += error * 7 / 16;
                    if x + 1 < width {
                        errors[y as usize + 1][x as usize + 1] += error * 3 / 16;
                    }
                }
            } else {
                if x >= 1 {
                    errors[y as usize][x as usize - 1] += error * 5 / 16;
                }
                if y + 1 < height {
                    if x >= 1 {
                        errors[y as usize + 1][x as usize - 1] += error * 3 / 16;
                    }
                    errors[y as usize + 1][x as usize] += error * 7 / 16;
                    if x + 1 < width {
                        errors[y as usize + 1][x as usize + 1] += error * 1 / 16;
                    }
                }
            }

            img.put_pixel(x, y, image::Rgba([new, new, new, rgba.channels()[3]]));
        }
    }
}

// main

fn print_usage() {
    println!("Usage: convert sample <output_file>");
    println!("Generate sample image.");
    println!("");
    println!("Usage: convert <mode> <input_file> <output_file>");
    println!("Convert image to black and white according to mode.");
    println!("Modes: 1 (round to nearest), 2 (random dithering), 3 (ordered dithering),");
    println!("       4 (forward error diffusion), 5 (forward and backward error diffusion),");
    println!("       6 (floyd's error diffusion)");
}

fn main() {
    let args: Vec<_> = env::args().collect();
    if args.len() < 2 {
        print_usage();
        return
    }

    if args[1] == "sample" {
        if args.len() != 3 {
            print_usage();
            return
        }
        let mut img = image::ImageBuffer::new(512, 512);
        for x in 0..512 {
            for y in 0..512 {
                img.put_pixel(x, y, image::Rgba([(x / 2) as u8, (x / 2) as u8, (x / 2) as u8, 255 as u8]));
            }
        }
        let ref mut out = File::create(&Path::new(&args[2])).ok().expect("Can't open output file");
        image::ImageRgba8(img).save(out, image::PNG).ok().expect("Can't save image");
        return
    }

    if args.len() != 4 {
        print_usage();
        return
    }

    let mut img = image::open(&Path::new(&args[2])).ok().expect("Can't open input file");

    match args[1].as_ref() {
        "1" => image_round(&mut img),
        "2" => image_random_dither(&mut img),
        "3" => image_ordered_dither(&mut img),
        "4" => image_diffuse_forward(&mut img),
        "5" => image_diffuse_double(&mut img),
        "6" => image_diffuse_floyd(&mut img),
        _ => {
            print_usage();
            return;
        }
    };

    let ref mut out = File::create(&Path::new(&args[3])).ok().expect("Can't open output file");
    img.save(out, image::PNG).ok().expect("Can't save image");
}
