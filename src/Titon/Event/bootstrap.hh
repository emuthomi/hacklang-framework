<?hh
/**
 * @copyright   2010-2015, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

/**
 * --------------------------------------------------------------
 *  Type Aliases
 * --------------------------------------------------------------
 *
 * Defines type aliases that are used by the event package.
 */

namespace Titon\Event {
    type CallStackList = Vector<string>;
    type EventMap = Map<string, Event>;
    type ListenerMap = Map<string, mixed>;
    type ListenerOption = shape('method' => string, 'priority' => int, 'once' => bool);
    type ObserverList = Vector<Observer>;
    type ObserverContainer = Map<string, ObserverList>;
    type ObserverCallback = (function(...): mixed);
    type ParamList = array<mixed>;
}