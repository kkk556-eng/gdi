from PIL import Image
import numpy as np

def pixelate(image, pixel_size=10):
    """Пикселизация: уменьшаем картинку, потом увеличиваем обратно"""
    small = image.resize(
        (image.width // pixel_size, image.height // pixel_size),
        resample=Image.NEAREST
    )
    return small.resize(image.size, Image.NEAREST)

def rgb_trail(image, shift=15, axis='x'):
    """
    RGB-трейн: сдвигаем каналы отдельно.
    shift – сдвиг в пикселях
    axis – 'x' (горизонталь) или 'y' (вертикаль)
    """
    img = np.array(image).astype(np.int16)
    h, w, c = img.shape
    result = np.zeros_like(img)
    
    if axis == 'x':
        # Красный сдвиг вправо, синий влево, зелёный по центру
        result[:, :, 0] = np.roll(img[:, :, 0], shift, axis=1)   # R
        result[:, :, 1] = img[:, :, 1]                            # G
        result[:, :, 2] = np.roll(img[:, :, 2], -shift, axis=1)  # B
    else:  # вертикальный сдвиг
        result[:, :, 0] = np.roll(img[:, :, 0], shift, axis=0)
        result[:, :, 1] = img[:, :, 1]
        result[:, :, 2] = np.roll(img[:, :, 2], -shift, axis=0)
    
    # Обрезаем вылезшие значения и переводим в uint8
    result = np.clip(result, 0, 255).astype(np.uint8)
    return Image.fromarray(result)

# === Пример использования ===
if __name__ == "__main__":
    # Открываем изображение (укажи свой путь или сделай скриншот)
    # Для скриншота можно использовать: from PIL import ImageGrab; img = ImageGrab.grab()
    img = Image.open("input.jpg")  # замени на своё изображение
    
    # Применяем пикселизацию
    pixelated = pixelate(img, pixel_size=15)
    
    # К пикселизованному изображению добавляем RGB-трейн
    final = rgb_trail(pixelated, shift=12, axis='x')
    
    # Сохраняем результат
    final.save("output_pixel_rgb.png")
    final.show()