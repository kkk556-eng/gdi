from PIL import Image
import numpy as np

def pixelate(image, pixel_size=10):
    
    small = image.resize(
        (image.width // pixel_size, image.height // pixel_size),
        resample=Image.NEAREST
    )
    return small.resize(image.size, Image.NEAREST)

def rgb_trail(image, shift=15, axis='x'):
   
    img = np.array(image).astype(np.int16)
    h, w, c = img.shape
    result = np.zeros_like(img)
    
    if axis == 'x':
       
        result[:, :, 0] = np.roll(img[:, :, 0], shift, axis=1)   # R
        result[:, :, 1] = img[:, :, 1]                            # G
        result[:, :, 2] = np.roll(img[:, :, 2], -shift, axis=1)  # B
    else:  # вертикальный сдвиг
        result[:, :, 0] = np.roll(img[:, :, 0], shift, axis=0)
        result[:, :, 1] = img[:, :, 1]
        result[:, :, 2] = np.roll(img[:, :, 2], -shift, axis=0)
    
   
    result = np.clip(result, 0, 255).astype(np.uint8)
    return Image.fromarray(result)


if __name__ == "__main__": from PIL import ImageGrab; img = ImageGrab.grab()
    img = Image.open("input.jpg")  #
   
    pixelated = pixelate(img, pixel_size=15)
    
    final = rgb_trail(pixelated, shift=12, axis='x')
    
    # Сохраняем результат
    final.save("output_pixel_rgb.png")
    final.show()
