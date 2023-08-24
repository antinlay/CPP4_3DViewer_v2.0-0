// сделать gif анимацию в openglwidget и предоставить выбор куда сохранить gif
// файл Answer :

// Share icon
// Для создания анимации GIF в  OpenGLWidget  вам потребуется использовать
// стороннюю библиотеку для создания GIF-файлов, такую как  gif-h . Вот пример
// кода, который может помочь вам выполнить это:

// 1. Установите библиотеку  gif-h  с помощью менеджера пакетов вашей
// операционной системы или скачайте и скомпилируйте ее вручную.

// 2. Включите заголовочный файл  gif.h  и добавьте ссылку на библиотеку  gif  в
// ваш проект.

// 3. Создайте экземпляр структуры  GifWriter  для записи анимации GIF и
// откройте файл для записи. Например: cpp
#include "gif.h"

void OpenGLWidget::createGifAnimation() {
  // Открытие файла для записи
  GifWriter gifWriter;
  GifBegin(&gifWriter, "animation.gif", width(), height(),
           10);  // 10 - задержка между кадрами в десятых долях секунды

  // Запись кадров анимации
  for (int i = 0; i < numFrames; i++) {
    // Рендеринг текущего кадра
    renderFrame(i);

    // Чтение пикселей из буфера кадра
    QImage image = grabFramebuffer();

    // Преобразование формата пикселей
    image = image.convertToFormat(QImage::Format_RGBA8888);

    // Запись кадра в файл GIF
    GifWriteFrame(&gifWriter, image.constBits(), width(), height(), 10);
  }

  // Завершение записи анимации GIF
  GifEnd(&gifWriter);
}
// В этом коде функция  createGifAnimation()  создает анимацию GIF, записывая
// каждый кадр в файл. Для каждого кадра вызывается функция  renderFrame() ,
// которая выполняет рендеринг текущего кадра в  OpenGLWidget . Затем пиксели
// текущего кадра считываются с помощью  grabFramebuffer()  и записываются в
// файл GIF с помощью функции  GifWriteFrame() .

// Обратите внимание, что  numFrames  - это переменная, содержащая количество
// кадров в анимации, и вы должны настроить ее в соответствии с вашими
// потребностями.

// 4. После создания анимации GIF вы можете предоставить пользователю выбор
// места сохранения файла, используя диалоговое окно  QFileDialog . Например:
// cpp
void OpenGLWidget::saveGifAnimation() {
  // Создание анимации GIF
  createGifAnimation();

  // Открытие диалогового окна для выбора места сохранения файла
  QString filePath = QFileDialog::getSaveFileName(this, "Save GIF Animation",
                                                  "", "GIF Files (*.gif)");

  // Сохранение анимации GIF в выбранный файл
  QFile::copy("animation.gif", filePath);
}
// В этом коде функция  saveGifAnimation()  вызывает функцию
// createGifAnimation()  для создания анимации GIF. Затем открывается диалоговое
// окно  QFileDialog , которое позволяет пользователю выбрать место сохранения
// файла. После выбора места сохранения, анимация GIF копируется в выбранный
// файл с помощью  QFile::copy() .

// Обратите внимание, что вам также потребуется настроить размер и задержку
// между кадрами в функции  GifBegin()  в соответствии с вашими потребностями.

// Убедитесь, что у вас есть все необходимые файлы и зависимости, чтобы успешно
// выполнить этот код.