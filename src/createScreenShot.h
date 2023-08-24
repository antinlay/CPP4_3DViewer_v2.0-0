// сделать скриншот объекта в openglwidget
// Answer :

// Share icon
// Для создания скриншота объекта в  OpenGLWidget  вам понадобится использовать
// функции OpenGL и Qt для захвата изображения и сохранения его в файл. Вот
// пример кода, который может помочь вам выполнить это: cpp
#include <QFileDialog>
#include <QImage>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>

void OpenGLWidget::captureScreenshot() {
  // Получение размеров OpenGL виджета
  int width = this->width();
  int height = this->height();

  // Создание буфера кадра для рендеринга
  QOpenGLFramebufferObject fbo(width, height);
  fbo.bind();

  // Очистка буфера кадра и рендеринг объекта
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  paintGL();

  // Чтение пикселей из буфера кадра
  QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
  QImage image(f->glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE));

  // Отмена привязки буфера кадра
  fbo.release();

  // Сохранение изображения в файл
  QString filePath = QFileDialog::getSaveFileName(this, "Save Screenshot", "",
                                                  "PNG Image (*.png)");
  if (!filePath.isEmpty()) {
    image.save(filePath);
  }
}
// В этом коде функция  captureScreenshot()  создает буфер кадра (
// QOpenGLFramebufferObject ), очищает его, рендерит объект в OpenGL виджете, а
// затем читает пиксели из буфера кадра с помощью функции  glReadPixels() .
// Затем изображение сохраняется в файл с помощью  QImage::save() .

// Вы можете вызвать функцию  captureScreenshot()  из вашего кода или привязать
// ее к событию или кнопке для создания скриншота объекта в  OpenGLWidget .
// Обратите внимание, что для корректной работы данного кода необходимо иметь
// активный контекст OpenGL.