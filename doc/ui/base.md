# Scene

![Scene](scene.svg)

## Canvas

Render target for glyphs

- Resizable

### Glyph

Base render element

- Visible
- Scalable

## Composition

Tree structured group of components

- Component as representative

### Component

Base UI element

- Visible
- Scalable
- Interactive

#### Panel

Arranged group of components

- Passive

##### Stack panel

Arranges components starting from anchor side in non-overlapping way

![Stack panel](stack_panel.svg)

#### Control

Functional component

- Active

##### State control

Maintains internal state, state change policy and displays a glyph as state visual representation

![State control](state_control.svg)

## Scalability

![Scalability](scalability.svg)

## Indentation

![Indentation](indentation.svg)

## Interactivity

- Mouse wheel
- Mouse left button down
- Mouse right button down
- Mouse move (track enabled)
- Mouse left button up
